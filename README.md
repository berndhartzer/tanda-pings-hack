# Tanda pings challenge

This is a solution to [Tanda's pings challenge](https://github.com/TandaHQ/work-samples/tree/master/pings%20(backend) using [HHVM](https://hhvm.com/), [Hack](http://hacklang.org/), [Docker](https://www.docker.com/), and [SQLite](https://www.sqlite.org/).

For this solution I decided to use HHVM, Hack and SQLite because they were all technologies I hadn't worked with before. The HHVM/Hack ecosystem has always intrigued me as I spend a lot of time writing PHP. I was interested to see how much of my PHP knowledge was transferable to Hack and to get a feel for HHVM, which is [diverging from it's PHP roots](https://hhvm.com/blog/2017/09/18/the-future-of-hhvm.html).

## My solution

One of my goals for this was to set up a workflow with docker which allowed me fully develop and run an application without installing any application dependencies on my machine. I was able to achieve this by changing some [Composer](https://getcomposer.org/) defaults, like changing the dependency installation directory (see `composer.json` and `public/index.php`). By doing this the dependencies are retrieved and installed when building the Docker image, and won't be overridden when mounting the application files for development. This slightly impractical approach came with it's own caveats; such as having to build a new Docker image every time I wanted to install a new package (thankfully for this project that was rarely needed). This also explains why the `.gitignore` file is missing most of the usual suspects you would find in a PHP project (e.g. `/vendor`).

## Run solution

I have included a script which will build a Docker image, run a container, run the `pings.rb` tests, and stop the container. From the application root directory, run:

```bash
$ ./bin/run.sh
```

_The `pings.rb` file is not run from a Docker container, so Ruby needs to be installed locally_

## Development

To develop this application locally, first you need to use the `Dockerfile` to build a Docker image:

```bash
# Run command from application root
$ docker build .
```

Once the image has been created, use the image id to run a Docker container with the local application files mounted:

```bash
$ docker run -v $(pwd):/var/www -d -p 3000:80 <image_id>
```

While thats running you can edit the application files (in `src/` and `public/`) using your favourite editor, IDE or tools. When you're finished you can stop running the container with:

```bash
$ docker stop <container_id>
```
