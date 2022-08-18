# Tranportation-App
This Command Application is made for both passenger and Driver . Here is some commands of ths app.

POST signup ? username <username> role <role>                             For signup
POST trips ? username <username> origin <origin_name> destination         For travel request(just passenjers can)
GET trips ? username <username> sort_by_cost <yes/no>                     To see list of travels(just drivers can)
GET trips ? username <username> id <trip_id>                              To see details of a trip(just drivers can)
DELETE trips ? username <username> id <trip_id>                           For canceling a trip(just passenjers can)
POST accept ? username <username> id <trip_id>                            For except a travel(just drivers can)
POST finish ? username <username> id <trip_id>                            For announcing the end of a trip


Also you can see persian description from here:
[Description(1).pdf](https://github.com/Aliparviz/Tranportation-App/files/9374117/Description.1.pdf)
