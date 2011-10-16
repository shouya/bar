#!/bin/bash

kill -9 `ps | grep demo | awk '{print $1}'` 
