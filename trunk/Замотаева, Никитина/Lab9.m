% EXAMPLE: Construct modulation objects
% to perform QPSK modulation and demodulation
% Modulator object
h = modem.pskmod('M', 4);
% Demodulator object
g = modem.pskdemod('M', 4);
% Modulating message
msg = randint(10,1,4);
% Modulate signal
modSignal = modulate(h,msg);
% Demodulate signal
demodSignal = demodulate(g,modSignal);