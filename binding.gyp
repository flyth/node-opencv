{
  "targets": [{ 
      "target_name": "opencv"
      , "sources": [ 
          "src/init.cc"
        , "src/Matrix.cc"
        , "src/OpenCV.cc"
        , "src/CascadeClassifierWrap.cc"
        , "src/Contours.cc"
        , "src/Point.cc"
        , "src/VideoCaptureWrap.cc"
        , "src/FaceRecognizer.cc"
        ]
      , 'libraries': [
          '<!@(pkg-config --libs opencv)'
        ]
      , 'cflags': [
            '<!@(pkg-config --cflags --libs opencv)'
            , '-Wall'
          ]
      , 'cflags!' : [ '-fno-exceptions']
      , 'cflags_cc!': [ '-fno-rtti',  '-fno-exceptions']
      , "conditions": [
        ['OS=="mac"', {
          # cflags on OS X are stupid and have to be defined like this
          'xcode_settings': {
            'OTHER_CFLAGS': [
              '<!@(pkg-config --cflags opencv)',
              '-I/usr/local/Cellar/opencv/2.4.3/include/opencv2'
            ]
            , "GCC_ENABLE_CPP_RTTI": "YES"
            , "GCC_ENABLE_CPP_EXCEPTIONS": "YES"
          }
        }]        
      
    ]
  }]
}

