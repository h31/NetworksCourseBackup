plot(x(1:200),y(1:200)) %�������� ������
grid;
figure
plot(t,u1)  %�������������� ������
figure
yy=u1.*cos(fc*t);
plot(t,yy);
ss=fft(yy,512);
sss=ss.*conj(ss)/512;
figure
plot(f,sss(1:256));
[b,a]=butter(4,0.9);
YY=filter(b,a,yy);
figure
plot(t,YY);
