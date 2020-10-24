<p align="center"><img src="https://raw.githubusercontent.com/jerome-Dh/etoile-bleue-server/master/www/favicon.png" width="72" alt="Icone"></p>

# Etoile Bleue Server

## About Etoile Bleue

Etoile Bleue is a lightweight web server that implements the HTTP / 2.0 protocol.
It was designed to perform hybrid parallel execution to reduce query latency and then deliver high performance.
This project was formalized in university research.

## How to use

The project is organized in several sub-directories.
To compile this you can just use the included makefile by running the command
``$ make all`` then run it with ``$. /server``

To change the port or host name, refer to the ``config.h`` file located in the ``headers`` directory before doing the compilation.
Many other configurations can be done by modifying this configuration file.

## Tests

When the server is running, it listens for all incoming connections on its port number, so you can use a browser such as Mozilla Firefox to try to get the `ʻindex.html`` page (this is the page by default provided by the server).

The ``www`` directory can be used to host other content that can be served by the server.

To stop the server, press the "Enter" key.

## Contributing

Your contribution is welcome, everyone can contribute by simply doing a PR.

## Author

[Jerome Dh](https://github.com/jerome-Dh/)
 
## License

This project is open-source software licensed under the [MIT license](https://opensource.org/licenses/MIT).