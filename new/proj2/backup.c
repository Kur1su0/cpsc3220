node_t* enQ(node_t* head, int priority){
    node_t* rover = head;
    if(rover == NULL) {
        //init thread.
        rover = malloc(sizeof(node_t));
        rover->priority = priority;
        pthread_cond_init(&(rover->waitCV), NULL);
        head->next = NULL;

    } else {
        int flag = 0;
        //Back up method...freezed for now...
        //node_t* tar = find_thread(lock->head, priority, &flag);
        if(flag==1) {
            //found thread. tar = desired thread.
            //TODO: Something about CV.

        } else {
            //Add to tail.
            tar->next = malloc(sizeof(node_t));
            tar->next->priority = priority;
            //Set up: 1.CV.
            tar->next->next = NULL;

        }


    }
