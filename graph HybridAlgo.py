import matplotlib.pyplot as plt

# Data for different algorithms
algorithms = ['Hybrid', 'FCFS', 'SJF', 'SRTF', 'RR']
waiting_time = [39.5, 43, 34, 34, 70.4]
turnaround_time = [65.5, 69, 60, 60, 96.4]
response_time = [26.5, 43, 34, 34, 26.8]

# Define colors for each category
colors = ['red', 'blue', 'green', 'orange', 'purple']

# Create a figure and axis
fig, ax = plt.subplots()

# Plot waiting time
ax.bar(algorithms, waiting_time, color=colors, label='Waiting Time')

# Plot turn-around time
ax.bar(algorithms, turnaround_time, color=colors, label='Turn-around Time')

# Plot response time
ax.bar(algorithms, response_time, color=colors, label='Response Time')

# Set axis labels and title
ax.set_xlabel('Scheduling Algorithm')
ax.set_ylabel('Time (ms)')
ax.set_title('Scheduling Algorithms Comparison')

# Add legend
ax.legend()

# Show the graph
plt.show()
