#!/bin/bash

read -p "Which city are you closest to? : " city
case $city in
    "New Yourk"|London|Paris|Tokyo)         echo "That is a capital city" ;;&
    Chicago|Detroit|"New York"|Washington)  echo "You are in the USA" ;;
    London|Edinburgh|Cardiff|Dublin)        echo "You are in the United Kingdom" ;;
    "Ramsey Street")                        echo "G'Day Neighbour!" ;&
    Melbourne|Canberra|Sydney)              echo "You are in Australia" ;&
    Pairs)                                  echo "You are in France" ;;
    Tokyo)                                  echo "You are in Japan" ;;
    N*)                                     echo "Your word begins with N but is not New York" ;;
    *)                                      echo "I'm sorry, I don't know anything about $city" ;;
esac
