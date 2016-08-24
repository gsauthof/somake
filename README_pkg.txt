`somake` is a portable version of illumos make. illumos make is
derived from Sun's dmake (distributed make) which in turn is
compatible with the bonus features of Sun make. Notable Sun make
features are:

- command dependencies (target is considered outdated if used
  variable has changed)
- hidden dependencies (automatic dependency generation for
  included headers)
- target groups
- conditional macro assignment (different syntax than with GNU
  make)
- include file generation (similar to GNU make)

See also: https://github.com/gsauthof/somake
