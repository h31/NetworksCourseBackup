f = 10;
%x = 0:1/(2*pi):8*pi;%4 periods
x = 0:0.01:8*pi;
y = sin(2*pi*f*x+pi/2);
plot(x(1:200),y(1:200))
grid
figure
spectrum = fft(y,512);
%spectrum = abs(fft(y,512));
norm_spectrum = spectrum.*conj(spectrum)/512;%спектр умн на его комплексное сопряженное - нормир спектр
f=100*(0:511)/512;
plot(f, norm_spectrum(1:512))
axis([0 max(f) 0 10])
grid

y_noise = y + 0.6 * rand(size(x));
figure
plot(x(1:200),y_noise(1:200));
%plot(x,y)
grid
spectrum_noise = fft(y_noise,512);
noise_spectrum = spectrum_noise.*conj(spectrum_noise)/512;
figure
plot(f, noise_spectrum(1:512))
axis([0 max(f) 0 10])
grid


%sizeof y is 158 ~ 160 
%Fd ~ 160/4 = 40 Hz
% синус не бесконечен! спектр исрытывает свертку с синком
% повторение спектра на частоте кратной частоте дискретезации