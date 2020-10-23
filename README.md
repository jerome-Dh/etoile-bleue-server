<p align="center"><img src="http://localhost:1977/favicon.png" width="72" alt="Icone"></p>

# Etoile Bleue Server

## About Etoile Bleue

Etoile Bleue is a lightweight web server that implements HTTP/2.0 protocol.
It has build to perform the hybrid parallel execution to slide down the latency and then give the high performancy.
This project has formalized in the university research purpose.

## How to use

The project is organize in several subdirectories.
To compile this, you can simplest use the included makefile by run the command 
``$ make all`` and then run it with ``$ ./server``

To change the port or hostname, refer to ``config.h`` file located in ``headers`` directory before make
the compilation. 
Many others configurations can be done by modifying this configuration file.

## Tests

When runing the server, it listen to all incoming connexions on his port number, as well you can use a navigator such Mozilla Firefox to try get the ``index.html`` page (this is the default page provide by server).

The ``www`` dir can be use to host others content that can be served by the server.

To stop the server, press ``Enter`` key.

## Contributing

Your contribution is welcome, everyone can contribute by simply make a PR.

## Author

[Jerome Dh](https://github.com/jerome-Dh/)
 
## License

This project is open-source software licensed under the [MIT license](https://opensource.org/licenses/MIT).