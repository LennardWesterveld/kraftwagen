<?php

/**
 * @file
 * This file contains the functions that are required to execute
 * `drush kw-composer-install`.
 */

/**
 * Implements drush_COMMAND_init() for `drush kw-composer-install`.
 */
function drush_kw_composer_install_init() {
  kraftwagen_context_init_kraftwagen_root(FALSE);
}

/**
 * Implements drush_COMMAND for `drush kw-composer-install`.
 *
 * @param string $location
 *   The location where the composer install should be generated.
 * @param string $src_dir
 *   The location of the src directory. Optional. If not specified the location
 *   will be figured out by determining the Kraftwagen root path.
 */
function drush_kraftwagen_kw_composer_install($location, $target_dir, $src_dir = NULL) {
  if (!$src_dir) {
    // Find out where the Kraftwagen root is.
    $root = kraftwagen_context_get_option('root-path');

    if (!($dir_src = kraftwagen_context_get_option('src-dir'))) {
      return drush_set_error(dt('No src dir name set.'));
    }

    if (!@lstat($root . DIRECTORY_SEPARATOR . $dir_src)) {
      return drush_set_error(dt('Could not find source at !src.', array('!src' => $root . DIRECTORY_SEPARATOR . $dir_src)));
    }

    $src_dir = $root . DIRECTORY_SEPARATOR . $dir_src;
  }

  if (!($dir_src_tools = kraftwagen_context_get_option('src-tools-dir'))) {
    return drush_set_error(dt('No src tools subdir name set.'));
  }

  require_once dirname(__FILE__) . '/includes/kraftwagen.fileutils.inc';

  //copy src to tmp
  drush_copy_dir($src_dir, $location, FILE_EXISTS_OVERWRITE);

  //build
  drush_op_system("/usr/local/bin/composer install -d " . $location);
  //TODO: check if gives status code 0
  
  //move tmp build folder
  drush_move_dir($location, $target_dir);


  drush_log(dt('Composer install runned (!path).', array('!path' => $location)), 'success');
}