#! /usr/bin/python

from time import strftime
import os
import re
from pprint import pprint
from datetime import timedelta, datetime
from matplotlib import pyplot as plt
import numpy as np

import sys

orig = 'rgb_blob.ppm'
sfc = 'sfc_blob.ppm'

def graph():

    lums = []
    with open(sfc, 'rb') as f:
        x = f.read(54);
        for x in range(400 * 400):
            b = f.read(1)
            if b:
                lums.append(int(b.encode('hex'), 16))
            else:
                break

    print(lums[:100])

    fig, ax = plt.subplots()
    ax.plot(lums)
    ax.set_xlim([55000, 70000])
    plt.text(
        0.5,
        1.08,
        "I will figure this out",
        fontsize=16,
        horizontalalignment='center',
        transform=ax.transAxes
    )

    ax.set_xlabel('Stream Position')
    ax.set_ylabel('Pixel Luminance')
    plt.show()

graph()
