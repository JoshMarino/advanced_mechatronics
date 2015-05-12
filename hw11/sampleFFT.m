port ='/dev/ttyUSB0';

% Makes sure port is closed
if ~isempty(instrfind)
    fclose(instrfind);
    delete(instrfind);
end
fprintf('Opening port %s....\n',port);
ser = serial(port, 'BaudRate', 230400, 'FlowControl', 'hardware');
fopen(ser);

% Generate the input signal
xp(1:50) = 200;
xp(51:100) = 0;
x = [xp xp xp xp xp xp xp xp xp xp 200*ones(1,24)];

% Create the FIR filter
Wn = 0.02; 
ord = 47; %ord+1 must be a multiple of 4 by the MIPS documentation
fir_coeff = fir1(ord,Wn);

N = length(x);
Y = fft(x); %compute MATLAB's fft

xfil = filter(fir_coeff,1,x); %filter the signal
Yfil = fft(xfil); %fft the filtered signal

% Generate data for FFT plots from the original signal
mag = 2*abs(Y(1:N/2+1))/N;
mag(1) = mag(1)/2;
mag(N/2+1) = mag(N/2+1)/2;

% Generate data for FFT plots for filtered signal
magfil = 2*abs(Yfil(1:N/2+1))/N;
magfil(1) = magfil(1)/2;
magfil(N/2+1) = magfil(N/2+1)/2;

freqs = linspace(0,1,N/2+1);

% Send the original signal to the pic32
fprintf(ser,'%d\n',N); %send the length
for i=1:N
    fprintf(ser,'%f\n',x(i)); %send each sample in the signal
end

% Send the fir filter coefficients
fprintf(ser,'%d\n',length(fir_coeff)); %send the length
for i=1:length(fir_coeff)
    fprintf(ser,'%f\n',fir_coeff(i));
end

% Read values sent from the PIC
Npic = fscanf(ser,'%d');
data = zeros(Npic,4) %columns: orig singal, fir filtered, orig fft, fir fft
for i=1:Npic
    data(i,:) = fscanf(ser,'%f %f %f %f');
end

xpic = data(:,1); %original signal from the pic
xfirpic = data(:,2); %fir filtered signal from pic
Xfftpic = data(1:N/2+1,3); %fft signal from pic
Xfftfir = data(1:N/2+1,4); %fft of filtered signal from pic

% Used to plot the fft pic signals
Xfftpic = 2*abs(Xfftpic);
Xfftpic(1) = Xfftpic(1)/2;
Xfftpic(N/2+1) = Xfftpic(N/2+1)/2;

Xfftfir = 2*abs(Xfftfir);
Xfftfir(1) = Xfftfir(1)/2;
Xfftfir(N/2+1) = Xfftfir(N/2+1)/2;

%Plot
subplot(3,1,1);
hold on;
title('Plot of the original signal and the FIT filtered signal')
xlabel('Sample number')
ylabel('Amplitude')
plot(x,'Marker','o');
plot(xfil,'Color','red','LineWidth',2);
plot(xfirpic,'*','Color','black');
hold off;
legend('Original Signal','MATLAB FIR','PIC FIR')
axis([-10,1050,-10,210])

subplot(3,1,2);
hold on;
title('FFTs of the original signal')
ylabel('Magnitude')
xlabel('Normalized frequency (fraction of Nyquist freq)')
stem(freqs,mag)
stem(freqs,Xfftpic,'Color','black')
legend('MATLAB FFT','PIC FFT')
hold off;

subplot(3,1,3);
hold on;
title('FFTs of filtered signal')
ylabel('Magnitude')
xlabel('Normalized frequency (fraction of Nyquist freq)')
stem(freqs,magfil)
stem(freqs,Xfftfir,'Color','black')
legend('MATLAB FFT', 'PIC FFT')
hold off;


% Closing serial connection
fclose(mySerial)