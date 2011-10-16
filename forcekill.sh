#!/bin/bash

kill -9 `ps | grep libbar | awk '{print $1}'` 
