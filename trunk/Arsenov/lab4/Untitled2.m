f = 15;
x = 0:0.01:8*pi;
y = sin(2*pi*f*x+pi/2);
plot(x(1:200),y(1:200))
grid
figure
spectrum = fft(y,512);

norm_spectrum = spectrum.*conj(spectrum)/512;
f=100*(0:511)/512;
plot(f, norm_spectrum(1:512))
axis([0 max(f) 0 10])
grid

y_noise = y + 0.6 * rand(size(x));
figure
plot(x(1:200),y_noise(1:200));
grid
spectrum_noise = fft(y_noise,512);
noise_spectrum = spectrum_noise.*conj(spectrum_noise)/512;
figure
plot(f, noise_spectrum(1:512))
axis([0 max(f) 0 10])
grid
