# A file for plotting the time it takes to build an index, using the result from the benchmark 
import matplotlib.pyplot as plt

# x axis values, n words
x1 = [100000, 200000, 400000, 800000, 1600000, 3200000, 6400000, 12800000, 
25600000, 51200000]
# corresponding y axis values, time (µs)
y1 = [16459, 27871, 51010, 108254, 194609, 482720, 806418, 1736615, 
3355498, 7131069]

y2 = [8154, 13743, 24989, 50104, 90157, 210053, 371630, 872114, 
1462308, 3739636]

y3 = [4802, 8220, 16318, 31102, 63001, 125823, 250563, 490311, 
984931, 2022749]

default_x1_ticks = range(len(x1))
default_y1_ticks = range(len(y1))
default_y2_ticks = range(len(y2))

plt.ticklabel_format(style='plain')
plt.plot(x1, y1, label = "Time/words with list of positions(µs)") 
plt.plot(x1, y2, label = "Time/words without list of positions(µs)") 
plt.plot(x1, y3, label = "Time/words with only trie (µs)") 




# naming the x axis
plt.xlabel('Number of words', fontsize = 15)
# naming the y axis
plt.ylabel('Time (µs)', fontsize = 15)

# giving a title to my graph
plt.title('Time inserting words to the index', fontsize = 25)

# show a legend on the plot
plt.legend()
# function to show the plot
plt.show()