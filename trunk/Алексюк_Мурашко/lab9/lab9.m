noise_level=30;
noise_persent = 20;
noise_persent = noise_persent/100;

%BPSK modulation
M=2;
N=10;
h = modem.pskmod('M', M);
g = modem.pskdemod('M', M);
msg = randint(N,1,M);
modSignal = modulate(h,msg);
% errSignal = (randerr(1,N, N*noise_persent) ./ noise_level)';
% modSignal = modSignal + errSignal;
demodSignal = demodulate(g,modSignal);
scatterplot(modSignal);
scatterplot(demodSignal);
errSignal = (randerr(1,N, N*noise_persent) ./ noise_level)';
modSignal = modSignal + errSignal;
demodSignal = demodulate(g,modSignal);
symerr(msg,demodSignal)/length(msg)*100

M=4;
N=10;
%PSK modulation
h = modem.pskmod('M', M);
g = modem.pskdemod('M', M);
msg = randint(N,1,M);
modSignal = modulate(h,msg);
% errSignal = (randerr(1,N, N*noise_persent) ./ noise_level)';
% modSignal = modSignal + errSignal;
demodSignal = demodulate(g,modSignal);
scatterplot(modSignal);
scatterplot(demodSignal);
errSignal = (randerr(1,N, N*noise_persent) ./ noise_level)';
modSignal = modSignal + errSignal;
demodSignal = demodulate(g,modSignal);
symerr(msg,demodSignal)/length(msg)*100

%OQPSK modulation
N=200;
M=4;
h = modem.oqpskmod('M',M);
g = modem.oqpskdemod(h);
msg = randint(N,1,M);
modSignal = modulate(h,msg);
% errSignal = (randerr(1,2*N, N*noise_persent) ./ noise_level)';
% modSignal = modSignal + errSignal;
demodSignal = demodulate(g,modSignal);
scatterplot(modSignal);
scatterplot(demodSignal);
errSignal = (randerr(1,2*N, N*noise_persent) ./ noise_level)';
modSignal = modSignal + errSignal;
demodSignal = demodulate(g,modSignal);
symerr(msg,demodSignal)/length(msg)*100

%GENQAM modulation
M = 10;
N = 10;
h = modem.genqammod('Constellation', exp(1i*2*pi*[0:M-1]/M));
g = modem.genqamdemod('Constellation', exp(1i*2*pi*[0:M-1]/M));
msg = randint(N,1,4);
modSignal = modulate(h,msg);
% errSignal = (randerr(1,N, N*noise_persent) ./ noise_level)';
% modSignal = modSignal + errSignal;
demodSignal = demodulate(g,modSignal);
scatterplot(modSignal);
scatterplot(demodSignal);
errSignal = (randerr(1,N, N*noise_persent) ./ noise_level)';
modSignal = modSignal + errSignal;
demodSignal = demodulate(g,modSignal);
symerr(msg,demodSignal)/length(msg)*100

N=20;
%MSK modulation
h = modem.mskmod('SamplesPerSymbol', N);
g = modem.mskdemod('SamplesPerSymbol', N);
msg = randint(N,1,2);
modSignal = modulate(h, msg);
% errSignal = (randerr(1,N*N, N*noise_persent) ./ noise_level)';
% modSignal = modSignal + errSignal;
demodSignal = demodulate(g, modSignal);
scatterplot(modSignal);
scatterplot(demodSignal);
demodSignal = demodulate(g,modSignal);
errSignal = (randerr(1,N*N, N*noise_persent) ./ noise_level)';
modSignal = modSignal + errSignal;
symerr(msg,demodSignal)/length(msg)*100

