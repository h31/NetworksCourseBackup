function lab4()
x = 0:0.01:4*pi;
t0 = 10;
%исходный сигнал
y_norm = sin(2*pi*t0*x);
plot(x(1:200),y_norm(1:200))
grid
%спектр исходного сигнала
figure
spectrum = fft(y_norm,512);
norm_spectrum = spectrum.*conj(spectrum)/512;
f=100*(0:255)/512;
plot(f, norm_spectrum(1:256))
axis([0 max(f) 0 10])
grid
%зашумленный сигнал
y_noize = y_norm+ 0.8*rand(size(x));
figure
plot(x(1:200),y_noize(1:200));
grid
%спектр зашумленного сигнала
spectrum = fft(y_noize,512);
noize_spectrum = spectrum.*conj(spectrum)/512;
figure
plot(f, noize_spectrum(1:256))
axis([0 max(f) 0 10])
grid
end

