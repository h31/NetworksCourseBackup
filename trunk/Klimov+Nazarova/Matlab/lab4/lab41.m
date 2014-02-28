f = 10;
x = 0:1/(2*pi):8*pi;%4 periods
y = sin(2*pi*f*x+pi/2);
plot(x,y)
grid
figure
%spectrum = fft(y,512);
spectrum = abs(fft(y,512));
norm_spectrum = spectrum.*conj(spectrum)/512;%спектр умн на его комплексное сопряженное - нормир спектр
f=100*(0:511)/512;
plot(f, norm_spectrum(1:512))
axis([0 max(f) 0 10])
grid
%sizeof y is 158 ~ 160 
%Fd ~ 160/4 = 40 Hz
% синус не бесконечен! спектр исрытывает свертку с синком
% повторение спектра на частоте кратной частоте дискретезации