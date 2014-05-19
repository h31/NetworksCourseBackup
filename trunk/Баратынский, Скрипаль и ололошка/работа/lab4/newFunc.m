function newFunc()
%t = 0: 1/(2*pi):4*pi;
t = 0:0.01:8*pi;
%y = sin(2*pi*5*t);
y = cos(2*pi*5*t+pi/2);
plot(t(1:200),y(1:200))
grid
figure
spectrum = fft(y,1024)
%norm_spectrum = spectrum.*conj(spectrum)/512
f=100*(0:255)/1024
plot(f,abs(fft(0,256)))%norm_spectrum(1:256))
axis([0 max(f) 0 10])
grid
end

