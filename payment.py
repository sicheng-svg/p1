"""
信用卡扣款流程模拟
Credit Card Payment Simulation
"""
import re
import random
from datetime import datetime
from typing import Dict, Tuple


class CreditCard:
    """信用卡类"""

    def __init__(self, card_number: str, cvv: str, expiry_date: str, cardholder_name: str):
        self.card_number = card_number
        self.cvv = cvv
        self.expiry_date = expiry_date
        self.cardholder_name = cardholder_name

    def validate(self) -> Tuple[bool, str]:
        """验证信用卡信息"""
        # 验证卡号格式（16位数字）
        if not re.match(r'^\d{16}$', self.card_number.replace(' ', '')):
            return False, "无效的卡号格式"

        # 验证CVV（3位数字）
        if not re.match(r'^\d{3}$', self.cvv):
            return False, "无效的CVV"

        # 验证有效期格式（MM/YY）
        if not re.match(r'^(0[1-9]|1[0-2])\/\d{2}$', self.expiry_date):
            return False, "无效的有效期格式"

        # 检查卡是否过期
        month, year = map(int, self.expiry_date.split('/'))
        current_date = datetime.now()
        expiry_year = 2000 + year

        if expiry_year < current_date.year or \
           (expiry_year == current_date.year and month < current_date.month):
            return False, "信用卡已过期"

        # 验证持卡人姓名
        if not self.cardholder_name or len(self.cardholder_name.strip()) < 2:
            return False, "无效的持卡人姓名"

        return True, "验证通过"


class PaymentProcessor:
    """支付处理器"""

    def __init__(self):
        self.transaction_history = []

    def process_payment(self, card: CreditCard, amount: float, currency: str = "CNY") -> Dict:
        """
        处理支付请求

        Args:
            card: 信用卡对象
            amount: 支付金额
            currency: 货币类型

        Returns:
            包含交易结果的字典
        """
        print(f"\n{'='*50}")
        print("开始处理支付请求...")
        print(f"{'='*50}")

        # 生成交易ID
        transaction_id = f"TXN{datetime.now().strftime('%Y%m%d%H%M%S')}{random.randint(1000, 9999)}"

        # 验证金额
        if amount <= 0:
            return self._create_transaction_result(
                transaction_id, False, "无效的支付金额"
            )

        print(f"交易ID: {transaction_id}")
        print(f"支付金额: {amount:.2f} {currency}")
        print(f"持卡人: {card.cardholder_name}")
        print(f"卡号: ****-****-****-{card.card_number[-4:]}")

        # 步骤1: 验证信用卡信息
        print("\n[步骤 1/5] 验证信用卡信息...")
        is_valid, message = card.validate()
        if not is_valid:
            print(f"❌ 验证失败: {message}")
            return self._create_transaction_result(
                transaction_id, False, message
            )
        print(f"✓ {message}")

        # 步骤2: 连接支付网关
        print("\n[步骤 2/5] 连接支付网关...")
        if not self._connect_to_gateway():
            return self._create_transaction_result(
                transaction_id, False, "无法连接到支付网关"
            )
        print("✓ 连接成功")

        # 步骤3: 检查信用额度
        print("\n[步骤 3/5] 检查信用额度...")
        if not self._check_credit_limit(card, amount):
            return self._create_transaction_result(
                transaction_id, False, "信用额度不足"
            )
        print("✓ 额度充足")

        # 步骤4: 执行扣款
        print("\n[步骤 4/5] 执行扣款...")
        if not self._deduct_amount(card, amount):
            return self._create_transaction_result(
                transaction_id, False, "扣款失败"
            )
        print(f"✓ 成功扣款 {amount:.2f} {currency}")

        # 步骤5: 生成交易记录
        print("\n[步骤 5/5] 生成交易记录...")
        result = self._create_transaction_result(
            transaction_id, True, "交易成功", amount, currency
        )
        self.transaction_history.append(result)
        print("✓ 交易记录已保存")

        print(f"\n{'='*50}")
        print("✓ 支付处理完成")
        print(f"{'='*50}\n")

        return result

    def _connect_to_gateway(self) -> bool:
        """模拟连接支付网关"""
        # 90%成功率
        return random.random() < 0.9

    def _check_credit_limit(self, card: CreditCard, amount: float) -> bool:
        """模拟检查信用额度"""
        # 模拟随机额度（5000-50000）
        available_limit = random.uniform(5000, 50000)
        return amount <= available_limit

    def _deduct_amount(self, card: CreditCard, amount: float) -> bool:
        """模拟执行扣款"""
        # 95%成功率
        return random.random() < 0.95

    def _create_transaction_result(self, transaction_id: str, success: bool,
                                   message: str, amount: float = 0,
                                   currency: str = "CNY") -> Dict:
        """创建交易结果"""
        return {
            "transaction_id": transaction_id,
            "timestamp": datetime.now().isoformat(),
            "success": success,
            "message": message,
            "amount": amount,
            "currency": currency,
            "status": "completed" if success else "failed"
        }

    def get_transaction_history(self):
        """获取交易历史"""
        return self.transaction_history


def main():
    """主函数 - 演示支付流程"""

    print("\n" + "="*50)
    print("信用卡支付系统 - 演示")
    print("="*50)

    # 创建支付处理器
    processor = PaymentProcessor()

    # 测试场景1: 正常支付
    print("\n【场景 1】正常支付")
    card1 = CreditCard(
        card_number="4532015112830366",
        cvv="123",
        expiry_date="12/28",
        cardholder_name="张三"
    )
    result1 = processor.process_payment(card1, 299.99, "CNY")
    print(f"\n结果: {result1['message']}")

    # 测试场景2: 过期卡
    print("\n\n【场景 2】使用过期卡")
    card2 = CreditCard(
        card_number="4532015112830366",
        cvv="123",
        expiry_date="01/20",  # 已过期
        cardholder_name="李四"
    )
    result2 = processor.process_payment(card2, 150.00, "CNY")
    print(f"\n结果: {result2['message']}")

    # 测试场景3: 无效CVV
    print("\n\n【场景 3】无效的CVV")
    card3 = CreditCard(
        card_number="4532015112830366",
        cvv="12",  # 只有2位
        expiry_date="12/28",
        cardholder_name="王五"
    )
    result3 = processor.process_payment(card3, 500.00, "CNY")
    print(f"\n结果: {result3['message']}")

    # 显示交易历史
    print("\n\n" + "="*50)
    print("交易历史记录")
    print("="*50)
    for idx, transaction in enumerate(processor.get_transaction_history(), 1):
        print(f"\n交易 #{idx}")
        print(f"  ID: {transaction['transaction_id']}")
        print(f"  时间: {transaction['timestamp']}")
        print(f"  状态: {transaction['status']}")
        print(f"  金额: {transaction['amount']:.2f} {transaction['currency']}")
        print(f"  消息: {transaction['message']}")


if __name__ == "__main__":
    main()
