function main()
x=0:0.01:4*pi;
f0 = 5;
y = sin(2*pi*f0*x);
figure(1)
plot(x(1:200),y(1:200))
grid
figure(2)
spectrum = fft(y,1024);
norm_spectrum = spectrum.*conj(spectrum)/1024;
f = 100*(0:1023)/1024;
%plot(f, norm_spectrum(1:1024))
plot(f,abs(fft(y,1024)))
axis([0 max(f) 0 10])
grid
end