clc
close all
clear all

Data = csvread('takashin_Homework_sample.csv', 1, 0);

Time = Data(:,1);
IR = Data(:,2);
RED = Data(:,3);

plot(Time, IR, 'b', Time, RED, 'r');
title('IR and RED signals against Time');

%%
FilterDegree = 20; LowFreq = (1/6); HighFreq = (1/3); SampleFreq = 50; FirstSample = FilterDegree*SampleFreq ;
hd = design(fdesign.bandpass('N,Fc1,Fc2',FilterDegree,LowFreq,HighFreq,SampleFreq),'butter');
fvtool(hd)

IRFiltered = filter(hd,IR);
REDFiltered = filter(hd,RED);

figure
plot(Time(FirstSample:end), IRFiltered(FirstSample:end), 'b', Time(FirstSample:end), REDFiltered(FirstSample:end), 'r');
title('Filtered IR and RED signals');
%%
SignalToUse = REDFiltered(FirstSample:end);
Threshold = 0.00;
MinPeakHeight = 1;
MinPeakDistance = 30;
figure, plot(Time(FirstSample:end), SignalToUse), hold on

[peakValues, indexes] = findpeaks(SignalToUse, 'THRESHOLD', Threshold, 'MINPEAKHEIGHT', MinPeakHeight, 'MINPEAKDISTANCE', MinPeakDistance);

plot(Time(indexes) + FilterDegree, peakValues + 0.05, 'k^', 'markerfacecolor', [1 0 0])
title('Signal with detected peaks');

%%
RespRate = zeros(size(indexes));
Window = 3;
if Window < 2 
    Window = 2;
end

for i= 1 : 1 : size(indexes) - Window + 1
    
    TimeStart = Time(indexes(i));
    TimeEnd = Time(indexes(i + Window - 1));
    RespRate(i) = Window / (TimeEnd - TimeStart);
    RespRate(i) = RespRate(i) * 60;
    
end

RespRate = RespRate(1:size(RespRate) - Window + 1);

figure, plot(RespRate)
title('Detected Respiration Rate');