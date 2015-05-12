function plotFFT(x)

if mod(length(x),2) == 1
    x = [x,0];
end

N = length(x);
X = fft(x);
mag(1) = abs(X(2))/N;
mag(N/2+1) = abs(X(N/2+1))/N;
mag(2:N/2) = 2*abs(X(2:N/2))/N;
freqs = linspace(0,1,N/2+1);
stem(freqs,mag);
axis([-0.05 1.05 -0.1*max(mag) max(mag)+0.1*max(mag)]);
xlabel('Frequency (as fraction of Nyquist frequency)');
ylabel('Magnitude');
title('Single-sided FFT Magnitude');
set(gca,'FontSize',18);

end