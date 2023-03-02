# Worker Pool

## Description
* Parent Process creates n child processes
* Each child executes a job for certain seconds
* Interprocess Communication between parent and children ( and reverse )
* Gracefull Shutdown

The user runs the programm and inserts in the command line the **number of workers he would like to have as the first parameter** 
and the **name of the file he/she would like to see the parent - child communication results as the second parameter**

## Compile and Run the Worker Pool
#### Using Docker
Running the docker commands below, this is what will be executed ( ./app 3 test.txt )
```
docker build -t final .
docker run -it final
```

#### Manual
* first parameter: number of workers
* second parameter: name of text file
```
gcc -o app app.c
./app [first parameter] [second parameter]
```

## Run Worker Pool Test
```
python3 test.py
```

### Authors
#### Kokkalis Konstantinos [LinkedIn](https://www.linkedin.com/in/konstantinos-kokkalis-806821221/)
