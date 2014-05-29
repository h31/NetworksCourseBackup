%BPSK modulation
h = modem.pskmod('M', 2);
g = modem.pskdemod('M', 2);
msg = randint(10,1,2)
modSignal = modulate(h,msg);
errSignal = (randerr(1,10, 3) ./ 30)';
modSignal = modSignal + errSignal;
demodSignal = demodulate(g,modSignal);
figure
subplot(3, 1, 1)
plot(1:10, msg(1:10))
title('Сообщение')
subplot(3, 1, 2)
plot(1:10, modSignal(1:10))
title('Модулированный сигнал')
subplot(3, 1, 3)
plot(1:10, modSignal(1:10))
title('Демодулированный сигнал')
scatterplot(modSignal);
eyediagram(demodSignal,10);

%PSK modulation
h = modem.pskmod('M', 8);
g = modem.pskdemod('M', 8);
msg = randint(10,1,8);
modSignal = modulate(h,msg);
errSignal = (randerr(1,10, 3) ./ 30)';
modSignal = modSignal + errSignal;
demodSignal = demodulate(g,modSignal);
figure
subplot(3, 1, 1)
plot(1:10, msg(1:10))
title('Сообщение')
subplot(3, 1, 2)
plot(1:10, modSignal(1:10))
title('Модулированный сигнал')
subplot(3, 1, 3)
plot(1:10, modSignal(1:10))
title('Демодулированный сигнал')
scatterplot(modSignal);
eyediagram(demodSignal,10);

%OQPSK modulation
h = modem.oqpskmod;
g = modem.oqpskdemod;
msg = randint(200,1,4);
modSignal = modulate(h,msg);
errSignal = (randerr(1,400, 100) ./ 30)';
modSignal = modSignal + errSignal;
demodSignal = demodulate(g,modSignal);
figure
subplot(3, 1, 1)
plot(1:200, msg(1:200))
title('Сообщение')
subplot(3, 1, 2)
plot(1:200, modSignal(1:200))
title('Модулированный сигнал')
subplot(3, 1, 3)
plot(1:200, modSignal(1:200))
title('Демодулированный сигнал')
scatterplot(modSignal);
eyediagram(demodSignal,200);

%GENQAM modulation
M = 10;
h = modem.genqammod('Constellation', exp(j*2*pi*[0:M-1]/M));
g = modem.genqamdemod('Constellation', exp(j*2*pi*[0:M-1]/M));
msg = randint(10,1,8);
modSignal = modulate(h,msg);
errSignal = (randerr(1,10, 3) ./ 30)';
modSignal = modSignal + errSignal;
demodSignal = demodulate(g,modSignal);
subplot(3, 1, 1)
plot(1:10, msg(1:10))
title('Сообщение')
subplot(3, 1, 2)
plot(1:10, modSignal(1:10))
title('Модулированный сигнал')
subplot(3, 1, 3)
plot(1:10, modSignal(1:10))
title('Демодулированный сигнал')
scatterplot(modSignal);
eyediagram(demodSignal,10);

%MSK modulation
h = modem.mskmod('SamplesPerSymbol', 10);
g = modem.mskdemod('SamplesPerSymbol', 10);
msg = randint(10,1,2);
modSignal = modulate(h, msg);
errSignal = (randerr(1,100, 3) ./ 30)';
modSignal = modSignal + errSignal;
demodSignal = demodulate(g, modSignal);
subplot(3, 1, 1)
plot(1:10, msg(1:10))
title('Сообщение')
subplot(3, 1, 2)
plot(1:10, modSignal(1:10))
title('Модулированный сигнал')
subplot(3, 1, 3)
plot(1:10, modSignal(1:10))
title('Демодулированный сигнал')
scatterplot(modSignal);
eyediagram(demodSignal,10);