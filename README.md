# webserv (@1337maroc)

> This project was code for MACOS. Here is the [subject][1].
>

*"This is when you finally understand why a URL starts with HTTP"*

![Alt text](https://github.com/Pyt45/web-server/blob/main/webserv_example.png)

## About

Webserv is a project of the mandatory part of the cursus that I made with my friend [medymik][2] and [yslati][3].
<br/><br/>It's only made in C++ and was about creating a web server able to handle one of the most famous web protocol, HTTP 1.1.
Our program is working with a configuration file, so you can setup the port, some routes, the allowed methods and some other basic paramaters for a webserver.
<br/>It's build based on UNIX sockets, and since it's an HTTP server you can access web pages using your favorite browser.

Our server handles the following HTTP features :
- The basics of HTTP protocol (status line / request line, a few headers, and payload).
- GET, POST and DELETE methods.
- HTTP response status codes (200, 404, ...).
- Error pages for the appropriate status code.
- CGI, like rendering a .php file in your browser thanks to php-cgi for example.

## Understanding the configuration file

Webserv need a configuration file that will be loaded at the beginning of the program, setting up all the necessary parameters. It works in a similar way than Nginx configuration file.
</br>It will allow you to setup the following parameters :
- `server` -- to define several virtual servers (with multiple server blocks).
- `server_name` -- to identify a specific server block thanks to host header.
- `port` -- to configure on which port the server will be listening.
- `error_page` -- to set a path for custom error pages.
- `location` -- to set parameters for a specific route.
- `index` -- to serve a default file if none match the requested URI.
- `root` -- to route the requested URI to another directory.
- `upload_path` -- to set a specific directory for upload requests.
- `autoindex` -- to list all files present in the targeted directory.
- `allowed_methods` -- to define the allowed methods for HTTP requests.
- `fastcgi_pass` -- to configure a cgi to execute for a specific file extenstion (php-cgi for .php files for example, node for *.js and python for *.py).

![Alt text](https://github.com/llefranc/42_webserv/blob/main/config_file_example.png)

Here is a short example of a configuration file, where webserv will : 
- :white_check_mark: : serve requests made on port 8080.
- :white_check_mark: : have custom error pages stored in `./webserv/server1.com/errors/test_changing_path` directory.
- :white_check_mark: : route any requests made to `/cgi/*` to `/webserv/server1.com/cgi_test` directory.
- :white_check_mark: : execute `/cgi/*.php` files with php-cgi program.
- :white_check_mark: : accept only POST requests for URIs targeting `/post`.

You can fing more examples in `config` directory. `server1.com` is a directory which allow you to test some cases / requests.

## Building and running the project

1. Download/Clone this repo

        git clone https://github.com/Pyt45/web-server

2. `cd` into the root directory and run `make`

        cd web-server
        make

3.  Run `webserv` with a configuration file.
	
		./webserv ./config/webServ.conf

4. Make `curl` request to webserv
	
		curl localhost:5000

5.	Access web pages using your browser.
	
		# Example of a valid URI to request with a browser, with the config file from step 3
		http://localhost:5000/cgi_test/index.php

[1]: https://github.com/Pyt45/web-server/blob/main/webserv.en.subject.pdf
[2]: https://github.com/medymik
[3]: https://github.com/yslati

