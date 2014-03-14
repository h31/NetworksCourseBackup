x = 0:0.01:4*pi;
f0 = 5; 
%исходный сигнал
for n=0:length(x)
y = cos(2*pi*f0*x*n);
end
plot(x(1:200),y(1:200)) 
grid 
%спектр исходного сигнала
figure
spectrum = fft(y,512);
norm_spectrum = spectrum.*conj(spectrum)/512; 
f=100*(0:255)/512;
plot(f, norm_spectrum(1:256))
axis([0 max(f) 0 10])
grid