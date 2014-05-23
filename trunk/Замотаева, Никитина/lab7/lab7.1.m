clear all;
close all;
f0=3;   %частота сигнала
fd=150; %частота дискретизации
fc=30;  %частота несущего колебания
x=0:0.01:4*pi;
y=0.5*sin(2*pi*f0*x);   %низкочастотный гармонический сигнал
plot(x(1:200),y(1:200))
grid;
figure
M1=0.3; %глубина модуляции 1,2,3
M2=1;
M3=1.3;
t=0:0.001:10;
Um=0.5;
f=100*(0:255)/512;
u1=(1+Um*M1*cos(f0*t)).*cos(fc*t); %модулированный сигнал
plot(t,u1)
figure
s1=fft(u1,512); %спектр модулированного сигнала
ss1=s1.*conj(s1)/512;
plot(f,ss1(1:256));
figure
u2=Um*(1+M2*cos(f0*t)).*cos(fc*t);
plot(t,u2)
figure
s2=fft(u2,512);
ss2=s2.*conj(s2)/512;
plot(f,ss2(1:256));
figure
u3=Um*(1+M3*cos(f0*t)).*cos(fc*t);
plot(t,u3)
figure
s3=fft(u3,512);
ss3=s3.*conj(s3)/512;
plot(f,ss3(1:256));

 plot(x(1:200),y(1:200)) %исходный сигнал
grid;
figure
u4=Um*M1*cos(f0*t).*cos(fc*t); %модуляция с подавлением несущей
plot(t,u4)
figure
s4=fft(u4,512);
ss4=s4.*conj(s4)/512;
plot(f,ss4(1:256));
