#! /usr/bin/env python

# See README.txt for information and build instructions.

from __future__ import print_function
import states_pb2
import sys


# Iterates though all people in the AddressBook and prints info about them.
def ListSamples(states):
  for sample in states.samples:
    print(sample)

states = states_pb2.Samples()

# Read the existing address book.
with open(sys.argv[1], "rb") as f:
  states.ParseFromString(f.read())

ListSamples(states)

