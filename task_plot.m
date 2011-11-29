y1 = [1673883,1732096,1625858,1656094,1596734,1634204,1606962,1548515,1798279,1643974]*10^(-6)
x1 = [1:10]
y2 = [1879299,1797272,1796053,1703749,1724871,1751962,1857275,1750329,1689085,1729141,1708703,1837096,1757167,1753000,1798236,1674251,1712257,1707986,1700922,1721762,1709855,1704438,1747379,1723848,1834773,1702618,1762251,1781052,1742432,1726802,1684529,1828135]*10^(-6)
x2 = [1:32]
y3 = [1926156,1861880,1723871,1760528,1754040,1745768,1795051,1751384,1783734,1685559,1678197,1761530,1753643,1804350,1711302,1723292,1751182,1684976,1741934,1737731,1703927,1723860,1696992,1896976,1689011,1713895,1808968,1697595,1785769,1720779,1714179,1716198]*10^(-6)
x3 = [1:32]

plot(x1,y1,'r')
hold on
plot(x2,y2,'m')
hold on
plot(x3,y3,'b')
title("Task 1 : link-state, cpu-time versus source-node")
xlabel("source-node")
ylabel("cpu-time")
legend("small-network","large-net-a","large-net-b")
axis([1 32 0 3], "autox")

y4 = [4,4,4,4,4,4,4,3,4,4]
y5 = ones(1,32)*6
y6 = ones(1,32)*8

figure
plot(x1,y4,'r')
hold on
plot(x2,y5,'m')
hold on
plot(x3,y6,'b')
title("Task 2 : distance-vector (with-initial-node), num-iterations versus initial-node")
xlabel("initial-node")
ylabel("num-iterations")
legend("small-network","large-net-a","large-net-b")
axis([1 32 0 10], "autox")

y7 = ones(1,10)*3
y8 = ones(1,32)*5
y9 = ones(1,32)*7

figure
plot(x1,y7,'r')
hold on
plot(x2,y8,'m')
hold on
plot(x3,y9,'b')
title("Task 2 : distance-vector (router), num-iterations versus initial-node")
xlabel("initial-node")
ylabel("num-iterations")
legend("small-network","large-net-a","large-net-b")
axis([1 32 0 8], "autox")
