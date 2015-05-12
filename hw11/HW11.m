fileID = fopen('accels.txt','r');
formatSpec = '%f';
A = fscanf(fileID,formatSpec);


% Plot FFT using code from DSP chapter
figure
plotFFT(A)


% Plot MAFs from Matlab and DPS chapter, as well as low-pass filter using fir1
figure
wts = [1/8;repmat(1/4,3,1);1/8];
yS = conv(A,wts,'valid');
subplot(2,2,1)
plot(yS)
title('MAF - 1/8,1/4,1/4,1/4,1/8')

maf=ones(13,1)/13;
subplot(2,2,2)
plot(conv(maf,A))
title('MAF - ones(13,1)/13')

lpf=fir1(12,0.2);
subplot(2,2,3)
plot(conv(lpf,A))
title('FIR - lowpass 0.2')

lpf=fir1(12,0.05);
subplot(2,2,4)
plot(conv(lpf,A))
title('FIR - lowpass 0.05')