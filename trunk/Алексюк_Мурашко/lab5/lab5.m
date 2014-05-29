close all;
clear all;
x = 0:0.01:4*pi;
f0 = 5;
%исходный сигнал
%for n=1:length(x)
%  y(n) = cos(2*pi*f0*n*x(n));
%end;
for n=1:length(x)
  if mod(n,20) < 10
      y(n) = 0;
  else
      y(n) = 1;
  end;
end;
figure
plot(x(1:200),y(1:200))
axis([-0.5 2.5 -0.5 1.5])
y = conv(y,y);
for n=1:length(x)
    y(n) = y(n)/n;
end;
figure
plot(x(1:200),y(1:200))
grid
%спектр исходного сигнала
figure
spectrum = fft(y,512);
norm_spectrum = spectrum.*conj(spectrum)/512;
f=100*(0:255)/512;
plot(f, norm_spectrum(1:256))
axis([0 max(f) 0 1000])
grid

clear all;

t = 0:00.1:4*pi;
N = 100;
y = 0;

% Полигармонический сигнал
y = sin(pi*t)+sin(3*pi*t)+sin(pi*0.3*t);
plot(t,y)
grid

figure
spectrum = fft(y,512);
norm_spectrum = spectrum.*conj(spectrum)/512;
f=100*(0:255)/512;
plot(f, norm_spectrum(1:256))
axis([0 max(f) 0 10])
grid

% Треугольный сигнал
figure
y2 = conv(square(t,20),square(t,20));
plot(t(1:100),y2(1:100));
grid

figure
spectrum = fft(y2,512);
norm_spectrum = spectrum.*conj(spectrum)/512;
f2=100*(0:255)/512;
plot(f2, norm_spectrum(1:256)/1000)
axis([0 max(f2) 0 50])
grid