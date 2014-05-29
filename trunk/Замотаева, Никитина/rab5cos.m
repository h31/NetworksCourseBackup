x = 0:0.01:4*pi;
f=100*(0:255)/512; 
%сигнал y=sin(x)+sin(3x)
figure
y1 = sin(2*pi*x)+sin(2*pi*3*x);  
plot(x(1:200),y1(1:200))   
grid 
%спектр сигнала y=sin(x)+sin(3x)
figure
s1 = fft(y1,512);
ss1 = s1.*conj(s1)/512;
plot(f,ss1(1:256))
grid 
%сигнал y=sin(x)+cos(x)
figure
y2 = sin(2*pi*x)+cos(2*pi*x);  
plot(x(1:200),y2(1:200))   
grid 
%спектр сигнала y=sin(x)+cos(x)
figure
s2 = fft(y2,512);
ss2 = s2.*conj(s2)/512;
plot(f,ss2(1:256))
grid 
%сигнал y=sin(x)+3sin(3x)+5sin(5x)
figure
y3 = sin(2*pi*x)+3*sin(2*pi*3*x)+5*sin(2*pi*5*x);  
plot(x(1:200),y3(1:200))   
grid 
%спектр сигнала y=sin(x)+cos(x)
figure
s3 = fft(y3,512);
ss3 = s3.*conj(s3)/512;
plot(f,ss3(1:256))
grid
