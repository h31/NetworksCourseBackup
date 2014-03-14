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
y = conv(y,y);
for n=1:length(x)
    y(n) = y(n)/n;
end;
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
