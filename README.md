# Web Server

## Overview

A simple web server written in C++. It can serve files, and also be programmed with routes if desired. The design needs an overhaul but overall it works well for my first attempt.

It also has websockets.

## Goal

My goal was to see if I could write my own rudimentary web server in C++ with minimal libraries or frameworks. 

In order to achieve this, I wanted to keep the design a simple as possible. All the server needed to do was receive requests and return responses without crashing. After achieveing that, I could then easily add other features later.

## Technical Detail

### Overall Design

The program comprises two main parts, the State and Tasks.
The State is a single object that holds all the data of the program. The Tasks are what read and write to the State.
In a sense, the Tasks are functions that transform the State. This makes it easy to add new tasks that add functionality.

This might seem bad design, but I didn't see a reason to break up the State into smaller pieces since most tasks needed to use almost every part of the State. So breaking it up into smaller parts seemed like it would add unnecessary boilerplate that would end up making it confusing.

Nevertheless, the State has several lists: connections, inbound_requests, outbound_responses, requests. Connections represents the clients currently connected to the server. inbound_requests represents HTTP requests received. outbound_responses represents HTTP responses ready to send. Requests represents parsed HTTP requests that are waiting to be processed.

The way the server works is there are tasks that work on each of these lists sequentially. For instance, the Reception Task checks if there are any pending connections and adds them to the Connections list. The Receive Requests Tasks checks each Connection in the Connections list that is still active, and reads and parses any data into an HTTP request object and appends that to the inbound_requests list. Then the Initialize Client Request Task looks at each request in the inbound_requests list and determines what kinds of request it is and if the server can fulfill it, if not, it generates a bad request object, whatever the result, a client request object is made and put onto the requests list. Some task then fulfills this request and marks it done. The Finalize Request Task then checks each request and if they are done, generates a HTTP response object and places that on the outbound_responses list. Finally, the Send Responses Task generates a HTTP response for each object on the list and sends them to the client.

The reason I like this design is that it is simple and extendable. There are obvious optimizations that could be made, namely, instead of having each task checking every object on the lists they are concerned with (eg. every request being checked if they are done), it could be better to have a separate list for each of their states. Also, after writing this program, I realized that a better way to handle the HTTP request -> Client Request -> HTTP response chain would be to have objects called say Processors, that would have a function called process, which would take a HTTP request and return HTTP response and they would be associated with a route, similar to how ExpressJS works. 
