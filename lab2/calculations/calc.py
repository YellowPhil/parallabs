import subprocess
import matplotlib.pyplot as plt
from random import randrange
from math import pow
from abc import ABC

AVG_RUNS = 30
MAX_SYSTEM_THREADS = 16
MAX_TEST_THREAD = 30


class Runner:
    def __init__(self, binary: str):
        self.binary = binary 

    def __avg_for_n_threads(self, thread_num: int):
        time_array = []
        for _ in range(AVG_RUNS): 
            seed = randrange(0, int(pow(2, 16)))
            process = subprocess.run([self.binary, str(thread_num), str(seed)], stdout = subprocess.PIPE, stderr = subprocess.PIPE)
            time = float(process.stderr.strip()) # read only info about time
            time_array.append(time)
        return sum(time_array) / len(time_array)

    def get_graph_points(self):
        thread_to_avg_map = []
        for i in range(1, MAX_TEST_THREAD):
            average_time = [i, self.__avg_for_n_threads(i)]
            thread_to_avg_map.append(average_time)

        return thread_to_avg_map


class BasicPlotter:




class ABCDataCore(ABC):
    def __init__(self, plot_data):
        self.experimental_data = plot_data

    def theoretical_average_time(self, x: int):
        pass

    def theoretical_efficiency(self, x: int):
        pass

    def theoretical_acceleration(self, x: int):
        pass

    def load_theoretical_average_time(self):
        x = [i[0] for i in self.experimental_data]
        y = [self.theoretical_average_time(i) for i in x]
        return x, y

    def load_theoretical_efficiency(self):
        x = [i[0] for i in self.experimental_data]
        y = [self.theoretical_efficiency(i) for i in x]
        return x, y

    def load_theoretical_acceleration(self):
        x = [i[0] for i in self.experimental_data]
        y = [self.theoretical_acceleration(i) for i in x]
        return x, y

    def load_practical_average_time(self):
        pass

    def load_practical_efficiency(self):
        pass

    def load_practical_acceleration(self):
        pass


class Lab1DataCore(ABCDataCore):
    def theoretical_average_time(self, x):
        base_point = self.experimental_data[0][1]
        if x < MAX_SYSTEM_THREADS:
            return base_point/x
        return base_point / MAX_SYSTEM_THREADS

    def theoretical_efficiency(self, x):
        if x <= MAX_SYSTEM_THREADS:
            return 1 
        return 1/(x - MAX_SYSTEM_THREADS)

    def theoretical_acceleration(self, x):
        if x <= MAX_SYSTEM_THREADS:
            return x 
        return MAX_SYSTEM_THREADS

    def load_practical_average_time(self):
        x = [i[0] for i in self.experimental_data]
        y = [i[1] for i in self.experimental_data]
        return x, y

    def load_practical_efficiency(self):
        base_point = self.experimental_data[0][1]
        x = [i[0] for i in self.experimental_data]
        y = [base_point/(i[1]*i[0])for i in self.experimental_data]
        return x, y

    def load_practical_acceleration(self):
        base_point = self.experimental_data[0][1]
        x = [i[0] for i in self.experimental_data]
        y = [base_point/i[1] for i in self.experimental_data]
        return x, y


class Visualizer:
    def __init__(self, binary_name: str):
        self.executor = ParallelExecutor(binary_name)
        self.experimental_data = self.executor.get_graph_experemental_info()
        self.data_core = Lab1DataCore(self.experimental_data)

    def visualize_average_time(self, name="AVG_time.png"):
        xt, yt = self.data_core.load_theoretical_average_time()
        xp, yp = self.data_core.load_practical_average_time()
        fig, ax = plt.subplots(layout='constrained')
        ax.plot(xt, yt, label="Theoretical AVG Time")
        ax.plot(xp, yp, label="Practical AVG Time")
        ax.set_xlabel("Threads")
        ax.set_ylabel("Time")
        ax.legend()
        fig.savefig(name)

    def visualize_acceleration(self, name="acceleration.png"):
        xt, yt = self.data_core.load_theoretical_acceleration()
        xp, yp = self.data_core.load_practical_acceleration()
        fig, ax = plt.subplots(layout='constrained')
        ax.plot(xt, yt, label="Theoretical Acceleration")
        ax.plot(xp, yp, label="Practical Acceleration")
        ax.set_xlabel("Threads")
        ax.set_ylabel("Acceleration")
        ax.legend()
        fig.savefig(name)

    def visualize_efficiency(self, name="efficiency.png"):
        xt, yt = self.data_core.load_theoretical_efficiency()
        xp, yp = self.data_core.load_practical_efficiency()
        fig, ax = plt.subplots(layout='constrained')
        ax.plot(xt, yt, label="Theoretical Efficiency")
        ax.plot(xp, yp, label="Practical Efficiency")
        ax.set_xlabel("Threads")
        ax.set_ylabel("Efficiency")
        ax.legend()
        fig.savefig(name)


def main():
    v = Visualizer('./a.out')
    v.visualize_average_time()
    v.visualize_efficiency()
    v.visualize_acceleration()

if __name__ == "__main__":
    main()

