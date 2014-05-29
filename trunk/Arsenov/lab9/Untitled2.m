h = modem.pskmod('M', 2);         %BPSK modulation
g = modem.pskdemod('M', 2);
message = randint(10,1,2)
modSignal = modulate(h,message);
errSignal = (randerr(1,10, 3) ./ 30)';
modSignal = modSignal + errSignal;
demodSignal = demodulate(g,modSignal);
figure
subplot(3, 1, 1)
plot(1:10, message(1:10))
title('���������')
subplot(3, 1, 2)
plot(1:10, modSignal(1:10))
title('�������������� ������')
subplot(3, 1, 3)
plot(1:10, modSignal(1:10))
title('���������������� ������')
scatterplot(modSignal);
eyediagram(demodSignal,10);


h = modem.pskmod('M', 8);             %PSK modulation
g = modem.pskdemod('M', 8);
message = randint(10,1,8);
modSignal = modulate(h,message);
errSignal = (randerr(1,10, 3) ./ 30)';
modSignal = modSignal + errSignal;
demodSignal = demodulate(g,modSignal);
figure
subplot(3, 1, 1)
plot(1:10, message(1:10))
title('���������')
subplot(3, 1, 2)
plot(1:10, modSignal(1:10))
title('�������������� ������')
subplot(3, 1, 3)
plot(1:10, modSignal(1:10))
title('���������������� ������')
scatterplot(modSignal);
eyediagram(demodSignal,10);


h = modem.oqpskmod;            %OQPSK modulation
g = modem.oqpskdemod;
message = randint(200,1,4);
modSignal = modulate(h,message);
errSignal = (randerr(1,400, 100) ./ 30)';
modSignal = modSignal + errSignal;
demodSignal = demodulate(g,modSignal);
figure
subplot(3, 1, 1)
plot(1:200, message(1:200))
title('���������')
subplot(3, 1, 2)
plot(1:200, modSignal(1:200))
title('�������������� ������')
subplot(3, 1, 3)
plot(1:200, modSignal(1:200))
title('���������������� ������')
scatterplot(modSignal);
eyediagram(demodSignal,200);


M = 10;             %GENQAM modulation
h = modem.genqammod('Constellation', exp(j*2*pi*[0:M-1]/M));
g = modem.genqamdemod('Constellation', exp(j*2*pi*[0:M-1]/M));
message = randint(10,1,8);
modSignal = modulate(h,message);
errSignal = (randerr(1,10, 3) ./ 30)';
modSignal = modSignal + errSignal;
demodSignal = demodulate(g,modSignal);
subplot(3, 1, 1)
plot(1:10, message(1:10))
title('���������')
subplot(3, 1, 2)
plot(1:10, modSignal(1:10))
title('�������������� ������')
subplot(3, 1, 3)
plot(1:10, modSignal(1:10))
title('���������������� ������')
scatterplot(modSignal);
eyediagram(demodSignal,10);

h = modem.mskmod('SamplesPerSymbol', 10);   %MSK modulation
g = modem.mskdemod('SamplesPerSymbol', 10);
message = randint(10,1,2);
modSignal = modulate(h, message);
errSignal = (randerr(1,100, 3) ./ 30)';
modSignal = modSignal + errSignal;
demodSignal = demodulate(g, modSignal);
subplot(3, 1, 1)
plot(1:10, message(1:10))
title('���������')
subplot(3, 1, 2)
plot(1:10, modSignal(1:10))
title('�������������� ������')
subplot(3, 1, 3)
plot(1:10, modSignal(1:10))
title('���������������� ������')
scatterplot(modSignal);
eyediagram(demodSignal,10);