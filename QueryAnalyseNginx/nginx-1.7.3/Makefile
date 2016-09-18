
default:	build

clean:
	rm -rf Makefile objs

build:
	$(MAKE) -f objs/Makefile
	$(MAKE) -f objs/Makefile manpage

install:
	$(MAKE) -f objs/Makefile install

upgrade:
	/data/fuyitao/QueryProcess/QueryAnalyseNginx/viw/nginx/sbin/nginx -t

	kill -USR2 `cat /data/fuyitao/QueryProcess/QueryAnalyseNginx/viw/nginx/logs/nginx.pid`
	sleep 1
	test -f /data/fuyitao/QueryProcess/QueryAnalyseNginx/viw/nginx/logs/nginx.pid.oldbin

	kill -QUIT `cat /data/fuyitao/QueryProcess/QueryAnalyseNginx/viw/nginx/logs/nginx.pid.oldbin`
