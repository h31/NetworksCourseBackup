function modulation()
x = 0:0.01:4*pi;
w = 1;
A =10;
y=A*cos(w*x);
figure(1)
plot(x,y);
grid on
f_0 = w/(2*pi);
mod_y=amod(y,f_0, 0.01);
figure(2)
plot(x,mod_y);
grid on

spectrum=abs(fft(mod_y));
%norm_spectrum=spectrum.*conj(spectrum)/1024;
f=100*(0:1023)/1024;
figure(3)
plot(f,spectrum(1:1024))
axis([8 12 0 max(spectrum)])
grid on

modc_y = amod(y,f_0,0.01);

figure(4)
plot(x,modc_y);
grid on
end