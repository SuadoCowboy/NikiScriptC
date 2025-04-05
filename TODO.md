- create a parser version that uses next() or something(see BindHandler::bind on JikaruEngine to have an idea)

- check all size limits everywhere

- use utf8? What would it do tho... Wouldn't the code already work with no issues?

- properly check conversions without using global errno variable cuz thread safety

- generic dynamic array functions: push, pop, clear