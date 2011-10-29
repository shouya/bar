#!/bin/bash

kill -9 `ps | grep -P 'demo|auto_ai' | awk '{print $1}'` 
