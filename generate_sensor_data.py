#!/usr/bin/env python3

import time
import random
import argparse
import numpy as np
from datetime import datetime, timedelta


# Initial timestamp for the sensor readings
initial_timestamp =  datetime.strptime("2024-04-18 10:00:00", '%Y-%m-%d %H:%M:%S')

# Parameters for generating the sensor readings
duration = 20 # seconds
sensors = {
    'hip_joint_angle':
    {
      'f_sampling': 100, # Hz
      'jitter': 0.02, # fraction of period
    },
    'hip_joint_gyro':
    {
      'f_sampling': 40, # Hz
      'jitter': 0.05, # fraction of period
    }
    }

# Parameters for generating the hip joint dynamics parameters
gait_cycle = 1.0 # seconds
gait_frequency = 1.0 / gait_cycle # Hz    

max_angle = 30*np.pi/180 # radians
min_angle = -5*np.pi/180 # radians



def generate_time_series(f_sampling: float, duration:float, initial_timestamp: datetime = datetime.now(), jitter: float = 0.0) -> np.ndarray:
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
    period = 1/f_sampling
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
    Generates dummy readings from a hip joint angle sensor.
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
    
    
    fundamental_freq = 2 * np.pi * gait_frequency
    #thirdharmonic_angle = 2 * np.pi * 3 * gait_frequency * time_series
    
    angle_data = amplitude*np.sin(fundamental_freq * time_series) + offset
    
    return angle_data

def generate_gyro_sensor(time_series: np.ndarray) -> np.ndarray:
    """
    Generates dummy readings from a 1-axis (hip joint) gyroscope from an IMU sensor data.
    Assumes a person walking with a gait cycle of 1.0 seconds.

    Parameters:
    time_series (numpy.ndarray): The time series for which to generate the angle sensor data.

    Returns:
    numpy.ndarray: The generated angle sensor data in radians.
    """
    
    amplitude = (max_angle - min_angle) / 2
    offset = (max_angle + min_angle) / 2
    
    
    fundamental_freq =  2 * np.pi * gait_frequency
    #thirdharmonic_angle = 2 * np.pi * 3 * gait_frequency * time_series
    
    angle_data = 2 * np.pi * gait_frequency * amplitude * np.cos(fundamental_freq * time_series)
    
    return angle_data



if __name__ == "__main__":
    parser = argparse.ArgumentParser(description='Generate dummy sensor data.')
    parser.add_argument('sensor_name', type=str, help='Name of sensor to generate data for.')
    
    args = parser.parse_args()

    if args.sensor_name not in sensors:
        raise ValueError(f'Unknown sensor name: {args.sensor_name}.')  

    f_sampling = sensors[args.sensor_name]['f_sampling']
    jitter = sensors[args.sensor_name]['jitter']
    time_series = generate_time_series(f_sampling, duration, initial_timestamp=initial_timestamp, jitter=jitter)

    if args.sensor_name == 'hip_joint_angle':
        data_series = generate_angle_sensor(time_series)
    elif args.sensor_name == 'hip_joint_gyro':
        data_series = generate_gyro_sensor(time_series)

    for (t, data) in zip(time_series, data_series):
        print(f'{t},{data}')
    
