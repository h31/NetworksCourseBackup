function main()
x = 0:0.1:100;
e = 0:0.1:100;
y = cos(x) + cos(3/4*pi*x);
%plot(fft(y));
%subplot(2,1,1), plot(y);
%subplot(2,1,2), plot(abs(fft(y,512)),'r');


c = [ 0 0 1 1 1 1 0 0];
b = [ 0 0 1 1 1 1 0 0];
subplot(2,1,1), plot(c);
subplot(2,1,2), plot(abs(fft(c)),'r');
%plot(conv(c,b));


end