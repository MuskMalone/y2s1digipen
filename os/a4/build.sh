if [ "$#" -lt 2 ]; then
    make $1
    make run
else
    make test$1
    make runtest
fi
