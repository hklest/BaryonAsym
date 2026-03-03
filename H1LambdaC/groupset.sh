pathmunge () {
    case ":${PATH}:" in
        *:"$1":*)
            ;;
        *)
            if [ "$2" = "after" ] ; then
                PATH=$PATH:$1
            else
                PATH=$1:$PATH
            fi
    esac
}

if  [[ $# == 1 ]]; then
  export GROUPROOT=/afs/desy.de/group/$1
    if [ -t 1 ]; then
       export ENVIRONMENT="LOGIN"
       if [ -r $GROUPROOT/etc/login.sh ];  then
         . $GROUPROOT/etc/login.sh 
       fi
    else
          export ENVIRONMENT=""
    fi
  
  for set in  env aliases ; do
    if [ -r $GROUPROOT/etc/$set.sh ];  then
       . $GROUPROOT/etc/$set.sh 
    fi
  done
  #pathmunge $GROUPROOT/scripts
  #pathmunge $GROUPROOT/bin
  GROUPPATH=`echo $GROUPPATH | awk -v OFS=":" '$1=$1'`
  pathmunge $GROUPPATH
  export PATH
fi

unset -f pathmunge
