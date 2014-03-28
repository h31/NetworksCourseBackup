function lab7()
x=0:0.01:4*pi;
w=2 ;
A=50;
y=A*cos(w*x);
figure(1)
plot(x,y);
grid on
f = w/(2*pi);
mod_y=amod(y,f,0.01);
figure(2)
plot(x,mod_y);
grid on
sp=abs(fft(mod_y));
figure(3)
plot(x,sp);
end