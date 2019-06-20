# srplab5
Synchronization multitasking in Windows environment
A program that has two streams: one prepares the data (for example, reads from the file), and the other sends them to the server. Here the streams works in turn. First, the first stream prepares a batch of data. Then the second stream sends it, and the first then prepares the next portion, etc. (for such a synchronization, two events will need to be "autoclaved").
