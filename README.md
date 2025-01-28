# Philoshophers

The Philosopher project is a classic multi-threading exercise based on the "Dining Philosophers" problem, which was introduced by Edsger Dijkstra. This problem illustrates synchronization issues in concurrent programming and requires careful resource sharing among multiple threads.

The objective is to create a program where a set of philosophers sit around a table with a fork between each pair. Each philosopher must alternate between thinking, eating, and sleeping, but they can only eat when they have two forks (one on their left and one on their right). The challenge lies in avoiding race conditions and deadlocks while ensuring efficient resource utilization.

![](https://i.imgur.com/896B72P.png)

- Command Line Arguments:

```bash
./philo <number_of_philosophers> <time_to_die> <time_to_eat> <time_to_sleep> <number_of_times_each_philosopher_must_eat>
```

`number_of_philosophers`: Number of philosophers (and forks).

`time_to_die`: Time (in milliseconds) a philosopher can live without eating.

`time_to_eat`: Time (in milliseconds) a philosopher takes to eat.

`time_to_sleep`: Time (in milliseconds) a philosopher takes to sleep.

`number_of_times_each_philosopher_must_eat` (optional): Simulation ends after each philosopher has eaten this many times.


