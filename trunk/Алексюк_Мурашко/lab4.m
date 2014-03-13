close all;
clear all;
x = 0:0.01:4*pi;
f0 = 5;
%исходный сигнал
y = sin(2*pi*f0*x);
plot(x(1:200),y(1:200))
grid
%спектр исходного сигнала 
figure
spectrum = abs(fft(y,128)); 
norm_spectrum = spectrum.*conj(spectrum)/512;
f=100*(0:127)/512;
plot(f, norm_spectrum(1:128))
axis([0 max(f) 0 10])
grid