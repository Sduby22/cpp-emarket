find . | egrep "(.\/src\/(server|client).*\.(cpp)$)|(\.\/include\/[^/]*\.h)" | xargs cat | wc -l
