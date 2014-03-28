close all;
clear all;
f0 = 10;
x = 0:0.01:8;
M = 0.5;
y = (1+M*cos(2*x)).*cos(50*x);
plot(x,y)
grid
spectrum = abs(fft(y,512));
%norm_spectrum = spectrum.*conj(spectrum)/512;
f=100*(0:511)/512;
figure
plot(f, spectrum)
axis([0 max(f) 0 250])
grid

y1 = (M*cos(2*x)).*cos(50*x);
figure
plot(x,y1)
grid
spectrum = abs(fft(y1,512));
%norm_spectrum = spectrum.*conj(spectrum)/512;
figure
plot(f, spectrum)
axis([0 max(f) 0 80])
grid

%сложить несколько косинусов с различн. множ-ми, и к-тами и вывести спектр