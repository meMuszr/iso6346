project('iso6346', 'c',
  version: '0.1',
  license: 'GPL')

inc = include_directories('include')
subdir('include')
subdir('lib')
subdir('src')
subdir('test')

pkg = import('pkgconfig')

pkg.generate(	 
  version : '0.1',
  name : 'libiso6346',
  filebase  : 'iso6346',
  description: 'Validate a ISO6346 compliant container number against the supplied check digit',
  libraries: [iso6346lib]
  )

