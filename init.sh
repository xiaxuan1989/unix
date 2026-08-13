#!/bin/bash
# Unix 初始化脚本

FILE_SYS_EDITOR_SRC='filesystem-editor'
FILE_SYS_SCANNER='filescanner'
FILE_SYS_EDITOR='fsedit'
FILE_SYS_TOOLS_BIN_DIR='tools/filesystem-editor/bin'

# usage: addenv env_name path, set env_name=path
# this function is probably used later
function addenv() {
  sed -i -e "/^export $1=.*/d" ~/.bashrc #delete variable env_name
  echo "export $1=`readlink -e $2`" >> ~/.bashrc # set env_name=path
  echo "Attention, this script will add environment variables into ~/.bashrc."
  echo "Please run 'source ~/.bashrc' to let these variables take effect."
  echo "If you use fish/zsh other than bash, set these environment variables manually."
}

function init() {
  project_root_dir=$(pwd)
  mkdir -p $FILE_SYS_TOOLS_BIN_DIR
  cd $FILE_SYS_EDITOR_SRC
  make # build the tools
  cd $project_root_dir #back to root dir
  cp -r "$project_root_dir/$FILE_SYS_EDITOR_SRC/target/$FILE_SYS_EDITOR" $FILE_SYS_TOOLS_BIN_DIR
  cp -r "$project_root_dir/$FILE_SYS_EDITOR_SRC/target/$FILE_SYS_SCANNER" $FILE_SYS_TOOLS_BIN_DIR
}


init
