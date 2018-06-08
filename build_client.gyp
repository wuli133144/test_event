{

 'targets':[

         {
            'target_name':'client_test',
            'type':'executable',
            'dependencies':[],
            'defines':[],
            'include_dirs':[
              './'
            ],
            'sources':[

                  'client.c',
            ],
            
            'conditions':[

             ['OS=="linux"',
                {
                'cflags':[
                   
                 ],
                'ldflags':[]
                },{
                  'cflags':[],
                  'ldflags':[],
                  'libraries':[
                    '-levent',
                    '-lpthread'
                   
                  ]
                }
                
             ]


            ]
           	
         } 
          
 ]
}

