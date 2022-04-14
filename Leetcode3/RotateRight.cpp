#include <iostream>

struct ListNode
{
    int val;
    ListNode* next;
    ListNode() : val(0), next(nullptr) {}
    ListNode(int x) : val(x), next(nullptr) {}
    ListNode(int x, ListNode* next) : val(x), next(next) {}
};

ListNode* RotateRight(ListNode* head, int k)
{
    if (k == 0 || !head || !head->next)
    {
        return head;
    }

    ListNode* last = head;

    int length = 1;

    while (last->next != nullptr)
    {
        length++;
        last = last->next;
    }

    k %= length;
    ListNode* newLast = head;

    for (int i = 0; i < length - k - 1; i++)
    {
        newLast = newLast->next == nullptr ? head : newLast->next;
    }

    last->next = head;
    head = newLast->next;
    newLast->next = nullptr;

    return head;
}

int main()
{
    ListNode l5(5);
    ListNode l4(4, &l5);
    ListNode l3(3, &l4);
    ListNode l2(2, &l3);
    ListNode l1(1, &l2);

    ListNode* head = &l1;
    ListNode* temp = head;

    while (temp != nullptr)
    {
        std::cout << temp->val << " ";
        temp = temp->next;
    }

    std::cout << std::endl;
    temp = RotateRight(head, 2);

    while (temp != nullptr)
    {
        std::cout << temp->val << " ";
        temp = temp->next;
    }

    return 0;
}