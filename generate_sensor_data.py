#!/usr/bin/env python3

import time
import random
import argparse
import numpy as np
from datetime import datetime, timedelta



def generate_time_series(frequency: float, duration:float, initial_timestamp: datetime = datetime.now(), jitter: float = 0.0) -> np.ndarray:
    """
    Generates a time series with a given frequency and duration, adding a specified amount of jitter.

    Parameters:
    frequency (float): The frequency of the time series in Hz.
    duration (float): The duration of the time series in seconds.
    initial_timestamp (datetime): The initial timestamp in epoch.
    jitter (float): The standard deviation of the Gaussian noise added to the time series as a fraction of the period.

    Returns:
    numpy.ndarray: The generated time series.
    """
    period = 1/frequency
    jitter_sd = period * jitter
    
    # with constant frequency
    time_index = np.arange(0, duration, period)

    # add jitter
    time_index += np.random.normal(0, jitter_sd, len(time_index))
    
    # add initial timestamp
    time_index += initial_timestamp.timestamp()
    
    return time_index


def generate_angle_sensor(time_series: np.ndarray) -> np.ndarray:
    """
    Generates a time series of the hip joint angle sensor data.
    Assumes a person walking with a gait cycle of 1.0 seconds.

    Parameters:
    time_series (numpy.ndarray): The time series for which to generate the angle sensor data.

    Returns:
    numpy.ndarray: The generated angle sensor data in radians.
    """
    
    gait_cycle = 1.0 # seconds
    gait_frequency = 1.0 / gait_cycle # Hz    
    
    max_angle = 30*np.pi/180 # radians
    min_angle = -5*np.pi/180 # radians
    
    amplitude = (max_angle - min_angle) / 2
    offset = (max_angle + min_angle) / 2
    
    
    fundamental_angle = 2 * np.pi * gait_frequency * time_series
    thirdharmonic_angle = 2 * np.pi * 3 * gait_frequency * time_series
    
    angle_data = amplitude*np.sin(fundamental_angle) + (0.2*amplitude)*np.sin(thirdharmonic_angle+0.1*np.pi)  + offset
    
    return angle_data


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Generate sensor data.')
    parser.add_argument('frequency', type=float, help='Frequency of data generation in Hz')
    parser.add_argument('duration', type=int, help='Duration of data generation in seconds')
    args = parser.parse_args()

    time_series = generate_time_series(args.frequency, args.duration)
    angle_data = generate_angle_sensor(time_series)
    
    for (t, angle) in zip(time_series, angle_data):
        print(f'{t},{angle}')
    