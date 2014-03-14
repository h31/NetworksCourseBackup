function laba_5()
x = 0:0.01:4*pi;
f0 = 5;
y = 0;
for i=1:1:100
    y=y+cos(i*x);
end
plot(x(1:100),y(1:100));
figure(1)
spectrum=fft(y,512);
norm_spectrum=spectrum.*conj(spectrum)/512;
f=100*(0:255)/512;
figure(2)
plot(f,norm_spectrum(1:256))
axis([0 max(f) 0 10])
grid 


figure(3)
y1=square(x,50)
plot(x(1:1000),y1(1:1000),'LineWidth',2);
ylim([-1.2,1.2]);
spectrum=fft(y1,512);
norm_spectrum=spectrum.*conj(spectrum)/512;
f1=100*(0:255)/512;
figure(4)
plot(f1,norm_spectrum(1:256))
axis([0 max(f1) 0 10])
grid 


y2=conv(y1,y1);
figure(5)
plot(x(1:1000),y2(1:1000),'LineWidth',2);
grid

spectrum=fft(y2,512);
norm_spectrum=spectrum.*conj(spectrum)/512;
f2=100*(0:255)/512;
figure(6)
plot(f2,norm_spectrum(1:256)/1000)
axis([0 max(f2) 0 10])
grid 
end