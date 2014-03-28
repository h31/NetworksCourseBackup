t=0:0.05:2*pi;
y=cos(pi*t);
%plot(y);
ssb=0.2*cos(11*pi*t)+0.2*cos(9*pi*t);
plot(ssb);
sp=abs(fft(ssb));
plot(sp);
