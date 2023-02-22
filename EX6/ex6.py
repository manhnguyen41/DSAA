import re
import numpy as np

def ReadFromFile(filename):
    with open(filename, "r") as f:
        content = f.read()
        doc = re.sub(r"[^a-zA-Z0-9]", " ", content.lower()).split()
        f.close()
        return doc

def CaculateBOW(wordset, doc):
    d_doc = dict.fromkeys(wordset, 0)
    for word in doc:
        d_doc[word] = doc.count(word)
    return d_doc

def BagOfWords(doc1, doc2):
    wordset = np.union1d(doc1, doc2)
    d_doc1 = CaculateBOW(wordset, doc1)
    d_doc2 = CaculateBOW(wordset, doc2)
    countDuplicate = 0
    for word in wordset:
        if d_doc1[word] > 0 and d_doc2[word] > 0:
            countDuplicate += 1
    return countDuplicate / wordset.size

# def GetKShingle(doc, k):
#     kwordset = []
#     for i in range(len(doc) - k + 1):
#         kwords = ""
#         for idx in range(k):
#             kwords += doc[i + idx] + " "
#         kwordset.append(kwords[:len(kwords) - 1])
#     return kwordset

def Read(filename):
    with open(filename, "r") as f:
        content = f.read()
        doc = re.sub(r"[^a-zA-Z0-9 ]", " ", content.lower())
        f.close()
        return doc

def GetKShingle1(doc, k):
    kwordset = []
    for i in range(len(doc) - k + 1):
        kwords = ""
        for idx in range(k):
            kwords += doc[i + idx]
        kwordset.append(kwords)
    return kwordset

def Shingle1(doc1, doc2, k):
    wordset1 = GetKShingle1(doc1, k)
    wordset2 = GetKShingle1(doc2, k)  
    wordset = np.union1d(wordset1, wordset2)
    d_doc1 = CaculateBOW(wordset, wordset1)
    d_doc2 = CaculateBOW(wordset, wordset2)
    countDuplicate = 0
    for word in wordset:
        if d_doc1[word] > 0 and d_doc2[word] > 0:
            countDuplicate += 1
    return countDuplicate / wordset.size

# def Shingle(doc1, doc2, k):
#     wordset1 = GetKShingle(doc1, k)
#     wordset2 = GetKShingle(doc2, k)  
#     wordset = np.union1d(wordset1, wordset2)
#     d_doc1 = CaculateBOW(wordset, wordset1)
#     d_doc2 = CaculateBOW(wordset, wordset2)
#     countDuplicate = 0
#     for word in wordset:
#         if d_doc1[word] > 0 and d_doc2[word] > 0:
#             countDuplicate += 1
#     return countDuplicate / wordset.size

if __name__ == "__main__":
    doc1 = ReadFromFile("input1.txt")
    doc2 = ReadFromFile("input2.txt")
    doc3 = Read("input1.txt")
    doc4 = Read("input2.txt")
    k = 5
    print("bag of words: ", round(BagOfWords(doc1, doc2), 2))
    # print(str(k) + "-shingle: ", Shingle1(doc1, doc2, k), 2)
    print(GetKShingle1(doc3, 5))
    print(GetKShingle1(doc4, 5))
    print(str(k) + "-shingle: ", round(Shingle1(doc3, doc4, k), 2))
    k = 10
    print(str(k) + "-shingle: ", round(Shingle1(doc3, doc4, k), 2))
    # print(CaculateBOW())